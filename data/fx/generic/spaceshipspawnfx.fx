#########################################################
##	template

FxInfo

Input  
	Inpname	HandL
End

Input  
	Inpname	HandR
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	Hips
End

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	hookup
		Type	local
		At	hips
		Geom	BoxRotate
	End

################################################################
##Electricity
#####################################################

	Event
		EName	Electricity01
		Type	local
		At	HandL
		Part1	EnemyFX/Rikti/RiktiSparksx.part
		Part2	EnemyFX/Rikti/RiktiSparks2x.part
		
		PMagnet	bottemGlobeLightning2
	End

	Event
		EName	Electricity02
		Type	local
		At	HandR
		Part1	EnemyFX/Rikti/RiktiSparksx.part
		Part2	EnemyFX/Rikti/RiktiSparks2x.part
		PMagnet	bottemGlobeLightning2
	End

	Event
		
		EName	Electricity03
		Type	local
		At	LarmR
		Part1	EnemyFX/Rikti/RiktiSparksx.part
		Part2	EnemyFX/Rikti/RiktiSparks2x.part
		
		PMagnet	bottemGlobeLightning2
	End

	Event
		EName	bottemGlobeLightning2
		Type	local
		At	hookup
		altpiv	7
		Part1	EnemyFX/Rikti/ShipRays.part
		Part2	EnemyFX/Rikti/RiktiGlobeLightningRandom2.part
		Part3	EnemyFX/Rikti/ShipStreakRays.part
		
	End

End

Condition
	On 	Time
	Time 	20	
	
	Event
		Type	Start
		At	Waist
		Bhvr	:SpaceShip_Camera_Shake.bhvr
		Sound rumble1_loop 200 200 .9
	End	
End

Condition
	On 	Time
	Time 	20

	Event
		EName	bottemGlobeLightning
		Type	local
		At	hookup
		altpiv	7
		
		Part1	EnemyFX/Rikti/RiktiGlobeLightningDown.part
		Part2	EnemyFX/Rikti/RiktiGlobeLightning.part
		Part3	EnemyFX/Rikti/RiktiGlobeLightningRandom.part
		Part4	EnemyFX/Rikti/RiktiBeam.part
		Sound Beamdown7 3000.0 3000.0 1.0
		
	End
		

End

Condition
	On 	Time
	Time 	300

	Event

		Ename	bottemGlobeLightning
		Type	Destroy

	End

	Event

		Ename	Electricity01
		Type	Destroy

	End

	Event

		Ename	Electricity02
		Type	Destroy

	End
	
	Event

		Ename	Electricity03
		Type	Destroy

	End

	Event

		Ename	bottemGlobeLightning2
		Type	Destroy

	End

	Event

		Ename	HookUp
		Type	Destroy

	End

End	


End	