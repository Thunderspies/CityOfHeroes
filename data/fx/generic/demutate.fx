#############################################################
## Demutate.fx
#############################################################

FxInfo

Lifespan 900

##############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Origin
		Bhvr	GENERIC\Demutate.bhvr
		Flags	AdoptParentEntity
	End
End

##############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		part2	POWERS\Healing\HealingHeadGlows.part
		Lifespan 45
	End
	Event
		Type	Local
		At	origin
		sound	Demutate_01
		Lifespan 45
	End


	Event
		Type	Local
		At	Chest
		part1	POWERS\Healing\Healing01a.part
		Lifespan 45
	End

	Event
		Type	Local
		At	Waist
		part1	POWERS\Healing\Healing01a.part
		Lifespan 45
	End

	Event
		Type	Local
		At	SpadR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet Hips
		Lifespan 45
	End

	Event
		Type	Local
		At	SpadL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet Hips
		Lifespan 45
	End

	Event
		Type	Local
		At	UArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 45
	End

	Event
		Type	Local
		At	UArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 45
	End

	Event
		Type	Local
		At	LArmL
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 45
	End

	Event
		Type	Local
		At	LArmR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 45
	End

	Event
		Type	Local
		At	WepL
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 45
	End

	Event
		Type	Local
		At	WepR
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		part1	POWERS\Healing\Healing01a.part
		Lifespan 45
	End

	Event
		Type	Local
		At	ULEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet KneeL
		Lifespan 45
	End

	Event
		Type	Local
		At	ULEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet KneeR
		Lifespan 45
	End

	Event
		Type	Local
		At	LLEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet FootL
		Lifespan 45
	End

	Event
		Type	Local
		At	LLEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet FootR
		Lifespan 45
	End

	Event
		Type	Local
		At	FootL
		part1	POWERS\Healing\HealingBodyGlows.part
		Lifespan 45
	End

	Event
		Type	Local
		At	FootR
		part1	POWERS\Healing\HealingBodyGlows.part
		Lifespan 45
	End
End


#############################################################

Condition
	On	Time
	Time	2

	Event
		EName	RiktiRings
		Type	start
		At	Chest
		ChildFX	ENEMYFX\Rikti\Child_RandomEllipseRings.fx
		Lifespan 35
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	start
		At	Chest
		Part2	ENEMYFX\Rikti\RiktiEnergyBubblingPurple.part
		Part3	ENEMYFX\Rikti\RiktiEnergyBubbling.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	start
		At	Chest
		Part2	ENEMYFX\Rikti\RiktiPortalSpecks2.part
		Lifespan 10
	End
End


#############################################################

End