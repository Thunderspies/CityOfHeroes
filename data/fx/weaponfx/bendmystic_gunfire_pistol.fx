#########################################################
##	
##

FxInfo

Lifespan	100

##################################
Condition
	On 	Time
	Time 	29

	
#	Event
#		EName 	Prime
#		Type	local
#		At	BendMystic
#		Geom	Pistol
#				
#	End
	
	Event
		EName 	tracer
		Type	start
		At	BendMystic

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound techpistol2 100.0 100.0 .97
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	BendMystic
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

##################################


End				