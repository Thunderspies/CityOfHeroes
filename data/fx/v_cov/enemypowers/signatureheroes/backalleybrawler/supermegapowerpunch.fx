#########################################################
##	Lightning Bolt
##
FxInfo

Lifespan	200

Condition
	On 	Time
	Time 	10

	Event
		EName 	hand1
		Type	Local 
		At	WepL
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part

		Part1	:BuildupRings.part
		Part2	:BuildupSparks.part
		Part3   :BuildupGlow.part

		Sound chargeUp_1 50 50 .8

	End


	Event
		EName 	hand2
		Type	Local 
		At	WepR
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Part4   POWERS\PowerPunch\powerpunchRings.part
		Part5	POWERS\PowerPunch\Largepowerpunch3.part

		Part1	:BuildupRings.part
		Part2	:BuildupSparks.part
		Part3   :BuildupGlow.part
	End

End

Condition
	On 	Time
	Time 	63

	Event
		EName 	handA
		Type	Local 
		At	WepR
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End

	Event
		EName 	handB
		Type	Local 
		At	WepL
		
		Part3   POWERS\PowerPunch\QuickRings.part

	End

End

Condition
	On 	Time
	Time 	70

	Event
		EName 	Impact
		Type	Posit
		At	Chest
		Lookat	Target
	End

	Event
		EName 	Explode
		Type	Local
		At	Impact
		Bhvr	behaviors/CameraShake05Subtle.bhvr
		BhvrOverride
			PositionOffset	0 0 5
		End

		Part   :ExplosiveFlash.part
		Part   :ExplosiveRings.part
		Part   :ExplosiveRays.part

		LifeSpan	5

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Explode
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

End	


Condition
	On 	Time
	Time 	200

	Event
		EName 	all
		Type	Destroy

	End

End


End		