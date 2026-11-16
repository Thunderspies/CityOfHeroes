#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 2

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		EName	MuzzleFlash
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Flare01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Flare02.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Sparks01.part
		Lifespan 4
	End

	Event
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			Shake          0.75
			ShakeFallOff   0.075
			ShakeRadius    64.0
			PositionOffset	-1 0 0
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas02.part
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			PositionOffset	-0 0 0
			PYRRotate	0 90 0
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas01B.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas02B.part
		Lifespan 4
	End

	Event
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			PositionOffset	-0 0 0
			PYRRotate	0 90 130
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas01B.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas02B.part
		Lifespan 4
	End

	Event
		Type	Start
		At	MuzzleFlash
		BhvrOverride
			PositionOffset	-0 0 0
			PYRRotate	0 90 -130
		End
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas01B.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\MuzzleFlash_Gas02B.part
		Lifespan 4
	End
End

#############################################################

End