#############################################################
## FireBallTarget1.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Input

	Inpname	Hips
End

#############################################################

Condition
	On 	time
	Time 	0
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
		Lifespan 70
		LifespanJitter 	20
	End

#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstOUT.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	time
	Time 	0
	Repeat  12

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

#	Event
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End


	Event
		EName 	Prime1
		Type	Local
		At	hips

		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part

	End

	Event
		EName 	Prime2
		Type	Local
		At	hips
		Part1	:InfernoExplosion.part

		BhvrOverride
			#PositionOffset		0 7 0

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	10
		End

		Lifespan	1
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	hips
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part


	End
#
	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
	End
#
End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End


Condition
	On 	Time
	Time 	80

	Event
		EName 	All
		Type	Destroy
	End


End


End