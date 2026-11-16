#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	200

Input  
	Inpname	Root
End

Input  
	Inpname	hips
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

#############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	60
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .8
	
		LifeSpan	60
	End


End

Condition
	On 	Time
	Time 	23

	Event
		EName 	attach
		Type	Local 
		At	Root
		Geom	RoottoChestAdjustment
		Sound Cinders3 80 80 .8
		
	End

	Event
		EName 	Prime
		Type	Local 
		At	attach
		altpiv	1
		Part1	:Cinders.part
				
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime2
		Type	Local 
		At	attach
		altpiv	1
		
		Part2	:CindersRing.part
		
	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	attach
		altpiv	1
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End

End
