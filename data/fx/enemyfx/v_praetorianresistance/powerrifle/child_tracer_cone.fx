#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 15

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ConeOffsets1
		Type	Local
		At	Origin
		BhvrOverride
			PYRRotate	0 90 0
			PYRRotateJitter	20 10 0
			Scale		80 80 80
		End
		Geom	FX_ConeOffsets
	End

	Event
		Ename	TargetOffset1
		Type	Start
		At	ConeOffsets1
		AltPiv	1
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Start
		At	Origin
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			StartJitter 		0.1 0.1 0.1
			TrackRate		6.0
		End
		LookAt	TargetOffset1
		Magnet	TargetOffset1
		Lifespan 15
	End

	Event
		Ename	Incendiary1
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\BulletTrail_Round01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\BulletTrail_Round02.part
		Lifespan 5
	End

	Event
		Ename	Incendiary2
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\BulletTrail_Trail01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\BulletTrail_Trail03.part
		Lifespan 10
	End
End

#############################################################

End