#############################################################
## ElectricBurst.fx
#############################################################

FxInfo
LifeSpan 500
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ArcCenter
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
	End

	Event
		Type	start
		At	ArcCenter
		Part	V_COV\Powers\ElectricalMelee\Blue_HitEmber.part
		Part	:ElectricBurst_Shockwave01.part
		Part	:ElectricBurst_Shockwave02.part
		Part	:ElectricBurst_Flash.part
		Part	V_COV\Powers\ElectricalMelee\HitStar.part
		Lifespan 100
	End

	Event
		Type	Local
		At	Root
		Sound 	Sewer_ElectricBurstHigh 100 100 .9
		Sound 	Sewer_ElectricBurstHigh_Loop 100 100 .9
		bhvr	behaviors\Sound\SoundIn01Out01.bhvr
		lifespan 80
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	start
		At	ArcCenter
		Part	:ElectricBurst_Arc_Curved01.part
		Part	:ElectricBurst_GlowFlash.part
		Lifespan 100
	End

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:ElectricBurst_ShockRing01_Ground.part
		Part	:ElectricBurst_GlowFlash_Ground.part
		Lifespan 100
	End
End
#############################################################
End