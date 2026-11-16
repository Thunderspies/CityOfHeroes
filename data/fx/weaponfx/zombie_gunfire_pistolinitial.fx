#########################################################
##	
##

FxInfo

Lifespan	144

##################################
Condition
	On 	Time
	Time 	77

	
	Event
		EName 	Prime
		Type	local
		At	WepR
		Geom	Pistol
				
	End
	
	Event
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		Sound Revolver2 100.0 100.0 .8
		LifeSpan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime
		altpiv	1
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End


End

##################################


Condition
	On 	Time
	Time	144

	Event
		Ename 	All
		Type	Destroy
	End
End


End				