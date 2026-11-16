#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Root
		sound Antimatter_Ray 200 200 .8
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Flash
		Type	Local
		At	C_Eyes
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 65
	End
End

Condition
	On	Time
	Time	25

	Event
		Ename	Flash
		Type	Local
		At	C_Eyes
		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part
		Lifespan 65
	End
End

#############################################################

Condition
	On	Time
	Time 	45

	Event
		Ename	TargetAnchor
		Type	Start
		At	T_Root
	End
	Event
		EName 	BeamGeo
		Type	Start
		At	C_Eyes
		Bhvr	behaviors/LightningBeam.bhvr

		Part	:Beam_Glow.part
		Part	:Beam_Glow_Additive.part
		geom	FX_ElectricBeam_Arc

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		POther	TargetAnchor

		LifeSpan		25
	End

	Event
		Ename	BeamHit
		Type	Local
		At	TargetAnchor
		Part	:Hit_Beam_Flash.part
		Part	:Muzzle_Buildup_Flash_Highlight.part

		Lifespan	26
	End

	Event
		Ename 	Prime
		Type	Start
		At	C_Eyes
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0
		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part

		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	50



	Event
		Ename 	Prime1
		Type	Start
		At	C_Eyes
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0

		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part


		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	55



	Event
		Ename 	Prime2
		Type	Start
		At	C_Eyes
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		5.0

		End

		Part	:Projectile_Trail_Ember_Small_Antimatter.part
		Part	:Projectile_Trail_Tendril_Small_Subtractive.part
		Part	:Projectile_Trail_Glow_Small_Subtractive.part


		Magnet	TargetAnchor
		LookAt	TargetAnchor
		Lifespan 40
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1Hit


	Event
		EName 	Prime1
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

#############################################################

End