# Steps
# 1. Copy <src\Marlin\Marlin> to <build_dir>.
# 2. Rename Marlin.ino in <build_dir>.
# 3. Copy files in <variant_dir> to <build_dir>.
# 4. Watch <variant_dir> and copy to <build_dir>.

# Title
$Title = "Fracktal Works Marlin Build Automation";
Write-Host $Title -ForegroundColor Green;
Write-Host ("PWD: " + $pwd) -ForegroundColor Green;

# Directories
$pwd = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition;
$dir_src = $pwd + "\src";
$dir_common = $dir_src + "\common";
$dir_marlin = $dir_src + "\Marlin\Marlin";
$dir_intermediate = $pwd + "\intermediate";
$dir_output = $pwd + "\output";
$variants = [string[]]@("Julia18_GLCD", "Julia18_GLCD_HB", "Julia18_RPI", "Julia18_RPI_E");

# Check for Marlin or Exit
if(!( Test-Path -Path $dir_marlin )) {
    Write-Host "Marlin not found." -ForegroundColor Red;
    Write-Host "Press any key to exit." -ForegroundColor Yellow;
    $Host.UI.RawUI.ReadKey('NoEcho, IncludeKeyDown');
    return;
}

# Check for variant sources
for ($i = 0; $i -lt $variants.Count ; $i++) {
    $temp = $dir_src + "\" + $variants[$i];
    if(!( Test-Path -Path $temp )) {
        Write-Host ("Variant source not found: " + $variants[$i]) -ForegroundColor Red;
        Write-Host "Press any key to exit." -ForegroundColor Yellow;
        $Host.UI.RawUI.ReadKey('NoEcho, IncludeKeyDown');
        return;
    }
}

# Make intermediate dir if not exists
if(!( Test-Path -Path $dir_intermediate )) {
    New-Item -ItemType directory -Path $dir_intermediate;
}

# Make output dir if not exists
if(!( Test-Path -Path $dir_output )) {
    New-Item -ItemType directory -Path $dir_output;
}

# Show variant choice
$build_choice = [System.Management.Automation.Host.ChoiceDescription[]] @("&GLCD", "GLCD_&HB", "&RPI", "RPI_&E", "Quit");
$build_option = $host.UI.PromptForChoice($Title , "Choose a build variant or quit.", $build_choice, 0);
if($build_option -eq 4) {
    Write-Host "Quitting" -ForegroundColor Green;
    return;
}

# Generate variant path
$build_variant = $variants[$build_option];
$dir_variant = $dir_src + "\" + $build_variant;
$dir_build = $dir_intermediate + "\" + $build_variant;
$file_ino = $build_variant + ".ino";
$path_ino = $dir_build + "\" + $file_ino;
$path_hex = $path_ino + ".mega.hex";


# Display paths
Write-Host $dir_src -ForegroundColor Blue;
Write-Host $dir_marlin -ForegroundColor Blue;
Write-Host $dir_intermediate -ForegroundColor Blue;
Write-Host $dir_variant -ForegroundColor Blue;
Write-Host $dir_build -ForegroundColor Blue;
Write-Host $path_ino -ForegroundColor Blue;

# Clear intermediate dir
Remove-Item -Path ($dir_intermediate + "\*") -Recurse;

# Make intermediate dir for variant
New-Item -ItemType directory -Path $dir_build;

# Copy Marlin files to intermediate
Copy-Item -Path ($dir_marlin + "\*") -Destination $dir_build -recurse -Force;

# Copy common files to intermediate, if exists
if( ( Test-Path -Path $dir_common)) {
    Copy-Item -Path ($dir_common + "\*") -Destination $dir_build -recurse -Force;
}

# Rename Marlin.ino to $build_variant
Rename-Item -NewName $file_ino -Path ($dir_build + "\Marlin.ino");

# Copy variant files to intermediate
Copy-Item -Path ($dir_variant + "\*") -Destination $dir_build -recurse -Force -Verbose;

# Open ino file
Start-Process $path_ino;
Write-Host "Opening ino file" -ForegroundColor Green;


$Watcher = New-Object IO.FileSystemWatcher $dir_variant, "*.*" -Property @{ 
    IncludeSubdirectories = $false
    NotifyFilter = [IO.NotifyFilters]'FileName, LastWrite'
}

$onCreated = Register-ObjectEvent $Watcher -EventName Changed -Action {
	$path = $Event.SourceEventArgs.FullPath
	
    if($path -eq $path_hex) {
		$path_hex_destination = $dir_output + "\" + $build_variant + "_mega_" + (Get-date -Format ddMMyyyy_hhmmss).ToString() + ".hex"
		Copy-Item -Path $path_hex -Destination $path_hex_destination -Force -Verbose;
	} else {
		Copy-Item -Path $path -Destination $dir_build -recurse -Force -Verbose;
	}
   #$name = $Event.SourceEventArgs.Name
   #$changeType = $Event.SourceEventArgs.ChangeType
   #$timeStamp = $Event.TimeGenerated
   #Write-Host "The file '$name' was $changeType at $timeStamp"
   #Write-Host $path
   #Move-Item $path -Destination $destination -Force -Verbose
}

# Copy hex to output or exit
# $copyHex = $host.UI.PromptForChoice($Title , "Copy hex to output?", [System.Management.Automation.Host.ChoiceDescription[]] @("&Yes", "&No"), 1);
# if ($copyHex -eq 0) {
	# $path_hex = $dir_build + "\" + $file_ino + ".mega.hex";
	# $path_hex_destination = $dir_output + "\" + $build_variant + "_mega_" + (Get-date -Format ddMMyyyy_hhmmss).ToString() + ".hex"
	
	# if(!( Test-Path -Path $path_hex )) {
	#	Write-Host "Hex not found." -ForegroundColor Red;
	# } else {
	#	Copy-Item -Path $path_hex -Destination $path_hex_destination -Force -Verbose;
	# }
# }
Write-Host "Done!" -ForegroundColor Green
$Host.UI.RawUI.ReadKey('NoEcho, IncludeKeyDown');