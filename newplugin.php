<?
echo "Enter directory / project name: ";
$name = trim(fgets(STDIN));

echo "class name: ";
$classname = trim(fgets(STDIN));

if($classname == "")
	$classname = $name;


copytemplate("vs6/plugins/template_dsp", "vs6/plugins/" . strtolower($name)  . ".dsp");
@mkdir("src/plugins/" . strtolower($name));
copytemplate("src/plugins/plugin.h.template", "src/plugins/" . strtolower($name)  . "/plugin.h");
copytemplate("src/plugins/plugin.cpp.template", "src/plugins/" . strtolower($name)  . "/plugin.cpp");
copytemplate("src/plugins/class.h.template", "src/plugins/" . strtolower($name)  . "/".strtolower($classname).".h");
copytemplate("src/plugins/class.cpp.template", "src/plugins/" . strtolower($name)  . "/".strtolower($classname).".cpp");

$f = file_get_contents("Makefile");
$f = str_replace("#NEWDEPENDENCY#", "plugin_" . strtolower($name) . " #NEWDEPENDENCY#", $f);
$f = str_replace("#NEWMKDIR#", "	@mkdir -p obj/plugins/" . strtolower($name) . "\n#NEWMKDIR#", $f);
$f = str_replace("#NEWTARGET#", "plugin_".strtolower($name).": obj/plugins/".strtolower($name)."/".strtolower($classname)."_win32.o obj/plugins/".strtolower($name)."/plugin_win32.o\n	@echo -e \"    \\033[1mLD\\033[1m\\t\\033[22;35m$@\\033[39m\"\n	@$(CXX) $(CFLAGS) $(LDFLAGS) -Llibs/lib -W1 --out-implib -shared -o plugins/".$dll.".dll $^ -lopengl32 -lglu32 -lplugin_base\n\n#NEWTARGET#", $f);


file_put_contents("Makefile",$f);


echo "Done!";



function copytemplate($from, $to)
{
	global $name;
	global $dll;
	global $classname;

	$f = file_get_contents($from);
	$f = str_replace("#NAMEL#", strtolower($name), $f);
	$f = str_replace("#NAME#", $name, $f);
	$f = str_replace("#DLL#", $dll, $f);
	$f = str_replace("#CLASS#", $classname, $f);
	$f = str_replace("#CLASSFILE#", strtolower($classname), $f);
	$f = str_replace("#CLASSU#", strtoupper($classname), $f);
	file_put_contents($to,$f);
}


?>