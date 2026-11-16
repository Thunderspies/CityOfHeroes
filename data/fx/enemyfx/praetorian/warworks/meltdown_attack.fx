#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Plasma_Charged_Blast_Hit 200 200 1
	End

	Event
		Type	Local
		At	Hips
		Part	:Hit_Flash_Small2.part
		Part	:Explosion_Fire_Rings_Small2.part
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Part	:SelfDestruct_Streaks_Small2.part
		Part	:Hit_Sparks.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	8



	Event
		Type	Local
		At	UArmL
		Part	:Hit_Flash_Small2.part
		Part	:Explosion_Fire_Rings_Small2.part
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Part	:SelfDestruct_Streaks_Small2.part
		Part	:Hit_Sparks.part

		Lifespan 1
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	LArmR
		Part	:Hit_Flash_Small2.part
		Part	:Explosion_Fire_Rings_Small2.part
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Part	:SelfDestruct_Streaks_Small2.part
		Part	:Hit_Sparks.part

		Lifespan 1
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
		#Sound	transformB 80 60 .86
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Shockwave_Small.part
		Part	:Explosion_Streaks_Small.part
		Part	:Explosion_Fire_Rings_Large.part
		Lifespan 4
	End


	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/Vanish.bhvr
		Flags 	AdoptParentEntity
		Part	:Explosion_Streaks_Thin_Small.part
		Part	:Explosion_Sparks_Small.part
		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Explosion_Fire_Rings_Small.part
		Lifespan 5
	End
End

#############################################################

End