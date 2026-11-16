#############################################################
## Gunfire_ADVAssaultRifle02_Medium.fx
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
		sound	RES_ArmCannon_Fire_01 100.0 80.0 .9
		LifeSpan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		#Sound	Assault3 120.0 30.0 1.0
	End
End

#############################################################

Condition
	On 	Time
	Time 	24

	Event
		Type	local
		At	WepR
		ChildFX	ENEMYFX\V_PraetorianResistance\HeavyHands\Child_HeavyBurst_MuzzleFlash.fx
		Lifespan 28
	End

	Event
		Type	local
		At	WepR
		ChildFX	ENEMYFX\V_PraetorianResistance\HeavyHands\Child_HeavyBurst_Tracers.fx
		Lifespan 28
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	WepL
		BhvrOverride
			PyrRotate	0 180 0
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\HeavyHands\Child_HeavyBurst_MuzzleFlash.fx
		Lifespan 28
	End

	Event
		Type	local
		At	WepL
		BhvrOverride
			PyrRotate	0 180 0
		End
		ChildFX	ENEMYFX\V_PraetorianResistance\HeavyHands\Child_HeavyBurst_Tracers.fx
		Lifespan 28
	End
End

#############################################################

End