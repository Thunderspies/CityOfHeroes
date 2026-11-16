###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################
	Condition
	On 		Time
	Time		0


		Event
			Ename SwordAnchor
			At Origin
			Type Local
			BhvrOverride
				Alpha		1
				PositionOffset	-0.2 -0.05 -0.1
				PYRRotate	-2 -5 0
			End
			ANIM	CustomWeapon_TitanWeapon_Flaming_Sword
		End


		Event
			EName	HiltAnchor
			At 	SwordAnchor
			AnimPiv	Hips
			Type 	Local

		End

		Event
			EName	HeadAnchor
			At 	SwordAnchor
			AnimPiv	Jaw
			Type 	Local
		End

		Event
			At 	SwordAnchor
			AnimPiv	Hips
			Type 	Local
			POther 	HeadAnchor
			Part :TitanWeapon_FlamingSword_Blade_Core.part
			Part :TitanWeapon_FlamingSword_Blade_Core_Chunks.part
			Part :TitanWeapon_FlamingSword_Flame_Bits.part
		End

		Event
			At 	SwordAnchor
			AnimPiv	Jaw
			Type 	Local
			POther 	HiltAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :TitanWeapon_FlamingSword_Blade_Highlights.part
			Part :TitanWeapon_FlamingSword_Blade_Core2.part
			Part :TitanWeapon_FlamingSword_Blade_Trail.part
		End

		Event
			At 	SwordAnchor
			AnimPiv	UArmL
			Type 	Local
			POther 	HiltAnchor
			Part 	:TitanWeapon_FlamingSword_Guard_Core_Solid.part
			Part 	:TitanWeapon_FlamingSword_Guard_Core.part
			Part 	:TitanWeapon_FlamingSword_Guard_Highlights.part
		End

		Event
			At 	SwordAnchor
			AnimPiv	UArmR
			Type 	Local
			POther HiltAnchor
			Part 	:TitanWeapon_FlamingSword_Guard_Core_Solid.part
			Part 	:TitanWeapon_FlamingSword_Guard_Core.part
			Part 	:TitanWeapon_FlamingSword_Guard_Highlights.part
		End

		Event
			At 	SwordAnchor
			AnimPiv	Neck
			Type 	Local
			POther 	HiltAnchor
			BhvrOverride
				PositionOffset	0.0 0.0 -0.9
			End
			Part 	:TitanWeapon_FlamingSword_Grip_Highlights.part
			Part	:TitanWeapon_FlamingSword_Grip_Core.part
		End
	End

###########################################################

End
