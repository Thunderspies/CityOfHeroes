#########################################################
## Rikti Heavy Assault Suit - Teleport Fade-In
#########################################################

FxInfo

LifeSpan 120

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Offset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 12 0
		End
	End

	Event
		EName 	Prime
		Type	Start
		At	Offset 
		Bhvr	ENEMYFX\Rikti\portalfadeout.bhvr
		Flags	AdoptParentEntity
	End
End

Condition
	On	Time
	Time	3

#	Event
#		EName	Ring
#		Type	Start
#		At	Offset
#		Part1	ENEMYFX\Rikti\RiktiPortalEllipse.part
#	End

	Event
		Ename	Upwards
		Type	Start
		At	Offset
		Part1	ENEMYFX\Rikti\UpwardStreaks.part
	End

	Event
		Ename	elec
		Type	Start
		At	Offset
		Part2	ENEMYFX\Rikti\RiktiPortalSpark.part
	End
End

Condition
	On	Time
	Time	6

	Event
		EName	AntiMatter
		Type	Start
		At	Offset
		Sound Materialize1 500.0 500.0 .5
		Part3	ENEMYFX\Rikti\RiktiPortalSpecks.part
		Part4	ENEMYFX\Rikti\RiktiPortalStreaks.part
		Part2	ENEMYFX\Rikti\PortalRays.part
		Part3	ENEMYFX\Rikti\PortalStreakRays.part
		#Part2	ENEMYFX\Rikti\RiktiPortalRings.part
	End
End

Condition
	On	Time
	Time	12
	Event
		EName	Ring2
		Type	Start
		At	Offset
		Part3	ENEMYFX\Rikti\RiktiPortalEllipsePulse.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring4
		Type	Start
		At	Offset
		Part3	ENEMYFX\Rikti\RiktiEllipsePulseShrinkAngles.part
	End
End

#########################################################

End				