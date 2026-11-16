#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 15

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
			TrackRate		5.0
		End
		LookAt	Target
		Magnet	Target
		Lifespan 300
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

Condition
	On 	PrimeHit

	Event
		Ename	ALL
		Type	Destroy
	End
End

#############################################################

End