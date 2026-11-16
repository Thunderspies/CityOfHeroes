#########################################################
##	
##

FxInfo

Lifespan	120

##################################

Condition
	On 	Time
	Time 	30
	
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
		Sound techpistol2 100.0 100.0 .97
		Sound techpistol4 100.0 100.0 .97
		Sound techpistol5 100.0 100.0 .97
		Sound techpistol5 100.0 100.0 .97
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
	
End

Condition
	On 	Time
	Time 	33
	
	Event
		EName 	Prime1
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
		At	Prime1
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound techpistol3 100.0 100.0 .97
		Sound Gunslingershot3c 100.0 100.0 .99
		Sound Revolver3 100.0 100.0 .89
		Sound techpistol4 100.0 100.0 .97
		Sound techpistol6 100.0 100.0 .97
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime1
		altpiv	2
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

Condition
	On 	Time
	Time 	38

	Event
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound techpistol6 100.0 100.0 .97
		Sound techpistol3 100.0 100.0 .97
		Sound techpistol2 100.0 100.0 .97
		Sound Revolver3 100.0 100.0 .9
		Sound Gunslingershot3c 100.0 100.0 .99
		Sound techpistol4 100.0 100.0 .97
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
	
End

Condition
	On 	Time
	Time 	41

	Event
		EName 	tracer
		Type	start
		At	Prime1
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound techpistol2 100.0 100.0 .97
		Sound techpistol6 100.0 100.0 .97
		Sound Gunslingershot3c 100.0 100.0 .99
		Sound Revolver3 100.0 100.0 .99
		Sound Revolver2 100.0 100.0 .99
		Sound techpistol6 100.0 100.0 .97
		Sound techpistol4 100.0 100.0 .97
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime1
		altpiv	2
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

Condition
	On 	Time
	Time 	43

	Event
		EName 	tracer
		Type	start
		At	Prime
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound Revolver2 100.0 100.0 .99
		Sound techpistol2 100.0 100.0 .97
		Sound techpistol6 100.0 100.0 .97
		Sound Gunslingershot3b 100.0 100.0 .99
		Sound techpistol4 100.0 100.0 .97
		Sound Gunslingershot3c 100.0 100.0 .99
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
	
End

Condition
	On 	Time
	Time 	47

	Event
		EName 	tracer
		Type	start
		At	Prime1
		Altpiv	1

		Part1	WEAPONFX/Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX/Gunfire_pistolsmoke.part
		Part3	WEAPONFX/Gunfire_pistolMuzzflashSmall.part
		#Part4	WEAPONFX/Gunfire_PistolShell.Part
		Sound Gunslingershot3c 100.0 100.0 .99
		Lifespan	4
	End
	
	Event
		EName 	shell
		Type	start
		At	prime1
		altpiv	2
		
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End

End

##################################


End				