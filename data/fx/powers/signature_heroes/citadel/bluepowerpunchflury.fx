#########################################################
##	Lightning Bolt
##
FxInfo

lifespan	60

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	mystic
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	:Bluepowerpunch1.part
		Part2	:Bluepowerpunch2.part
		Part3	:Bluepowerpunch3.part
		Part4	:Bluepowerpunch2.part
		Part5   :BluepowerpunchGlow.part
		Sound powerup2 100 100 .8
	End


	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	:Bluepowerpunch1.part
		Part2	:Bluepowerpunch2.part
		Part3	:Bluepowerpunch3.part
		Part4	:Bluepowerpunch2.part
		Part5   :BluepowerpunchGlow.part
	End

		Event
		EName 	Prime3
		Type	Local 
		At	WepL
		
		Part3   :BluepowerpunchRings.part
	End
	

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		
		Part3   :BluepowerpunchRings.part

	End
End


Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	Local 
		At	mystic
		Part1	:BlueHitRingFlat.part
		Part2	:BlueHitRingThinFlat.part
		#Part3	:Bluepowerpunch3.part
		#Part4	:Bluepowerpunch2.part
		#Part5   :BluepowerpunchGlow.part
		#Sound 	powerup2 100 30 .8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
#####################################################################################

End


Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Destroy
	End

	Event
		EName 	Prime4
		Type	Destroy
	End

End

End		