#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 400

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Chest
		Part	:SelfDestruct_Flash_Big.part
		Part	:Hit_Glow.part
		Part	:Hit_Shockwave.part
		Part	:SelfDestruct_Glow_Lingering_Small.part
		Part	:SelfDestruct_Streaks_Small_Yellow.part
		Part	:Hit_Fire_Blue.part
		Part	:SelfDestruct_Smoke_Small2.part
		Sound WW_TheHarderTheyFall_Hit 200 200 .5
		Lifespan 5
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:SelfDestruct_Smoke_Small2.part
		Lifespan 5
	End
End

Condition

	On	Time
	Time	12

	Event
		Type	PositOnly
		At	SpadL
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Part	:SelfDestruct_Smoke_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	PositOnly
		At	ULegR
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	33

	Event
		Type	PositOnly
		At	LArmL
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	41

	Event
		Type	PositOnly
		At	SpadR
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	44

	Event
		Type	PositOnly
		At	Neck
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	48

	Event
		Type	PositOnly
		At	WepL
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	54

	Event
		Type	PositOnly
		At	SpadR
		Part	:SelfDestruct_Smoke_Small.part
		Part	:SelfDestruct_Flash.part
		Part	:SelfDestruct_Sparks.part
		Part	:SelfDestruct_Streaks_Small.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	29


	Event
		Type	PositOnly
		At	LLegL
		Bhvr	behaviors/CameraShake02Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle_Lingering_Light.part
		Part	:Dust_Circle_Lingering_Light2.part
		Lifespan 5
	End

	Event
		Type	PositOnly
		At	LLegR
		Bhvr	behaviors/CameraShake02Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle_Lingering_Light.part
		Part	:Dust_Circle_Lingering_Light2.part
		Lifespan 5
	End


End

Condition
	On	Time
	Time	80

	Event
		EName	HipsAnchor
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	-1.0 0.0 0.0
		End
	End

	Event
		Type	StartPositOnly
		At	HipsAnchor
		Bhvr	behaviors/CameraShake01.bhvr
		Part	:Dust_Circle_Large.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Shockwave_Large.part
		Part	:Dust_Shockwave_Fill.part
		Part	:Dust_Flash.part
		Part	:Sparks_Flash.part
		Lifespan	1
	End
End



Condition
	On	Time
	Time	180

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		ChildFX	:HarderTheyFall_Attack.fx
	End
End

#############################################################

End