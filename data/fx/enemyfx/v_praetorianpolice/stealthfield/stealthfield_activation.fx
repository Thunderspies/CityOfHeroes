#############################################################
## InvisStealth.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound rumble1 80 80 0.8
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At 	Origin
		Sound invisible1 80 80 0.5
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmR
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmL
		Lifespan 0
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmR
		Lifespan 0
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LarmL
		Lifespan 0
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Neck
		Lifespan 0
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Chest
		Lifespan 0
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Ulegl
		Lifespan 0
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	UlegR
		Lifespan 0
	End
End

Condition
	On	Time
	Time	40

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	Llegl
		Lifespan 0
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	POWERS\Invisibility\InvisBodyGlow02.part
		POther	LlegR
		Lifespan 0
	End
End

## FORM SHIELD MATRIX ###########################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_Chest
		Lifespan 26
	End
End

Condition
	On 	Time
	Time	8

	Event
		Type	Local
		At	UArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_UArmR
		Lifespan 18
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_UArmL
		Lifespan 18
	End
End

Condition
	On 	Time
	Time	14

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LArmR
		Lifespan 12
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LArmL
		Lifespan 12
	End
End

Condition
	On 	Time
	Time	8

	Event
		Type	Local
		At	ULegR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_ULegR
		Lifespan 16
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_ULegL
		Lifespan 16
	End
End

Condition
	On 	Time
	Time	16

	Event
		Type	Local
		At	LLegR
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LLegR
		Lifespan 10
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	ENEMYFX\V_PraetorianPolice\FormShield\FormShield_QuickHitFade.bhvr
		Geom	FX_FormShield_LLegL
		Lifespan 10
	End
End

#############################################################

End