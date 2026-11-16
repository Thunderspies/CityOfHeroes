#########################################################
##	
##

FxInfo

Lifespan	130

##################################
Condition
	On 	Time
	Time 	57
	
	Event
		EName 	point
		Type	local
		At	wepR

		Geom	Arachnos_Pistol02
	End
	
	Event
		EName 	tracer
		Type	start
		At	point
		altpiv	1
		
		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound heavypistol1 100.0 100.0 1.0
		LifeSpan 5
	End
	
	Event
		EName 	shell
		Type	start
		At	point
		altpiv	1
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

##################################



End				