#########################################################
##	
##

FxInfo

Lifespan	100

##################################

Condition
	On 	Time
	Time 	29
	
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
		Sound Gunslingershot4 100.0 100.0 .99
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime
		altpiv	2
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End
	
	Event
		EName 	Prime2
		Type	local
		At	WepL
		Geom	Pistol
		Bhvr	Behaviors/Reverse_wep.bhvr
		BhvrOverride
			PositionOffset	.1 0 0
		End
				
	End
	
	Event
		EName 	tracer
		Type	start
		At	Prime2
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		#Sound 	Revolver3 100.0 30.0 .8
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime2
		altpiv	2
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

##################################


End				