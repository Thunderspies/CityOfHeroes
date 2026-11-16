#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Head
End

LifeSpan	50

########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Energy
		Type	Local 
		At	Head
		
		Part1	:Stamina.part
		Part2	:StaminaSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part

	End

	Event
		EName	dummy
		Type	Local 
		At	Root
		Geom	CenterDummy
		Part1	:StaminaRingUp.part
	End
	
	Event
		EName	Ringup
		Type	Local 
		At	dummy
		altpiv	1
		bhvr	behaviors/UpWardsMotion5.bhvr
		Part1	:StaminaRing.part

	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Ringup2
		Type	Local 
		At	dummy
		altpiv	1
		bhvr	behaviors/UpWardsMotion3.bhvr
		Part1	:StaminaRing.part

	End
End

Condition
	On	Time
	Time	7

	Event
		EName	dummy
		Type	Local 
		At	Root
		Geom	CenterDummy
		Part1	:StaminaRingUp.part
	
	End
End

Condition
	On	Time
	Time	7

	Event
		EName	dummy
		Type	Local 
		At	Root
		Geom	CenterDummy
		Part1	:StaminaRingUp.part
		Part2	:StaminaRingUp.part
	End

	Event
		EName	Ringup3
		Type	Local 
		At	dummy
		altpiv	1
		bhvr	behaviors/UpWardsMotion4.bhvr
		Part1	:StaminaRing.part

	End

End

Condition
	On	Time
	Time	10

	Event
		EName	RingUp
		Type	Destroy
	End

End

Condition
	On	Time
	Time	18

	Event
		EName	RingUp2
		Type	Destroy
	End
	
	Event
		EName	Energy
		Type	Destroy
	End

End

Condition
	On	Time
	Time	26
	
	Event
		EName	RingUp3
		Type	Destroy
	End

End

End
##################################

