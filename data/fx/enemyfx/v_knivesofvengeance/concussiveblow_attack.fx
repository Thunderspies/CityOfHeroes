#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 800

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak5
		Type	local
		At	FootR
		Sound footstomp2 100.0 100.0 .85
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	WepR
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	WepR
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Dust_Core_Flat.part
		Part	:Dust_Shockwave.part
		Part	:Light_Shockwave.part

		Lifespan	1
	End


End

Condition
	On 	Time
	Time 	34

	Event
		Type	StartPositOnly
		At	WepR
		bhvr    :Dust_Expanding.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Dust_Lingering.part
#		Part	:Dust_Expanding.part
#		Part	:Dust_Expanding_Thick.part
#		Part	:Dust_Flash.part
		Part	:Dust_Shockwave_Fill.part
		Part	:SpeedLines.part
		Part	:Sparks.part
#		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Light_Flash.part

		Lifespan	60
	End

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.0 4.0 0.0
		End
		Lifespan	20
	End

	Event
		Type	StartPositOnly
		At	WepR
		Part	:Dust_Pillar.part
		POther	DustAnchor
		Lifespan	1
	End

	Event
		EName	crack
		Type	Local
		At	Root
		Bhvr	CustomizeablePowers\SuperStrength\Splat_FootStomp.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 3.0
		End
		Splat	ImpactCracks.tga
		Lifespan 30
	End
End
#############################################################

End