#############################################################
## PowerSink_EnduranceGlow.fx
#############################################################

FxInfo
Lifespan 200

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	chest
		bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\Healing\enduranceBodyGlows2a.part
		Lifespan 28
	End

	Event
		EName	2
		Type	Local
		At	FootR
		Part	Powers\Healing\enduranceBodyGlowsa.part
		PMagnet LLEGR
		Lifespan 28
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		Part	Powers\Healing\enduranceMotionGlowsa.part
		PMagnet LLEGL
		Lifespan 28
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	Powers\Healing\enduranceMotionGlowsa.part
		PMagnet FootL
		Lifespan 28
	End

	Event
		EName	5
		Type	Local
		At	FootL
		Part	Powers\Healing\enduranceBodyGlowsa.part
		PMagnet LLEGL
		Lifespan 28
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	Powers\Healing\enduranceMotionGlowsa.part
		PMagnet LlegR
		Lifespan 28
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	Powers\Healing\enduranceMotionGlowsa.part
		PMagnet FootR
		Lifespan 28
	End

	Event
		EName	9
		Type	Local
		At	Hair
		Part	Powers\Healing\enduranceHeadGlowsa.part
		Lifespan 28
	End

	Event
		EName	10
		Type	Local
		At	UARML
		Part	Powers\Healing\enduranceArmMotionGlowsa.part
		PMagnet LARML
		Lifespan 28
	End

	Event
		EName	11
		Type	Local
		At	LARML
		Part	Powers\Healing\enduranceArmMotionGlowsa.part
		PMagnet Chest
		Lifespan 28
	End

	Event
		EName	12
		Type	Local
		At	Chest
		Part	Powers\Healing\enduranceArmGlowsa.part
		Lifespan 28
	End

	Event
		EName	13
		Type	Local
		At	UARMR
		Part	Powers\Healing\enduranceArmMotionGlowsa.part
		PMagnet LARMR
		Lifespan 28
	End

	Event
		EName	14
		Type	Local
		At	LARMR
		Part	Powers\Healing\enduranceArmMotionGlowsa.part
		PMagnet WepR
		Lifespan 28
	End

	Event
		EName	15
		Type	Local
		At	WepR
		Part	Powers\Healing\enduranceArmGlowsa.part
		Lifespan 28
	End
End

#############################################################

End