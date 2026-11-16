#############################################################
## RobotLazerBursts.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On Time
	Time 5
	
	Event
		Type	Local
		At	WepR
		sound	RES_Pistols_FireSnapShot_01 100.0 80.0 .9
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	MuzzleFlashOffset
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.5 0 0.625
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	34

	Event
		Type	Local
		At	MuzzleFlashOffset
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	Local
		At	MuzzleFlashOffset
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_MuzzleFlash_01.fx
		Lifespan 30
	End
End

## PROJECTILE 1 ###########################################################

Condition
	On 	Time
	Time 	28

	Event
		Ename	Prime
		Type	Start
		At	MuzzleFlashOffset
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	Prime
	End
End

## PROJECTILE 2 ###########################################################

Condition
	On 	Time
	Time 	36

	Event
		Ename	Prime2
		Type	Start
		At	MuzzleFlashOffset
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	Prime2Hit

	Event
		Type	Destroy
		Ename	Prime2
	End
End

## PROJECTILE 3 ###########################################################

Condition
	On 	Time
	Time 	44

	Event
		Ename	Prime3
		Type	Start
		At	MuzzleFlashOffset
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	Prime3Hit

	Event
		Type	Destroy
		Ename	Prime3
	End
End

#############################################################

End