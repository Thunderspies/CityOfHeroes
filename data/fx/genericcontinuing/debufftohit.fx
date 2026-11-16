#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Chestnode
		Type	Local
		At	Origin
		Geom	ChestPivotsBuffToHit
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:DeBuffToHitGlow01.part
		part	:DeBuffToHitCrosshair01.part
		part	:DeBuffToHitCrosshair02.part
		part	:DeBuffToHitCrosshair03.part
		Lifespan 60
	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:DeBuffToHitGlow01.part
		part	:DeBuffToHitCrosshair01.part
		part	:DeBuffToHitCrosshair02.part
		part	:DeBuffToHitCrosshair03.part
		Lifespan 60
	End

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:DeBuffToHitGlow01.part
		part	:DeBuffToHitCrosshair01.part
		part	:DeBuffToHitCrosshair02.part
		part	:DeBuffToHitCrosshair03.part
		Lifespan 60
	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:DeBuffToHitGlow01.part
		part	:DeBuffToHitCrosshair01.part
		part	:DeBuffToHitCrosshair02.part
		part	:DeBuffToHitCrosshair03.part
		Lifespan 60
	End
End

#############################################################

End