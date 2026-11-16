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
				PositionOffset	0.0 0.0 -0.15
			End
			ANIM	CustomWeapon_Katana_Flaming_Sword_01
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
			AnimPiv	Head
			Type 	Local
		End

		Event
			At 	SwordAnchor
			AnimPiv	Hips
			Type 	Local
			POther 	HeadAnchor
		#	Bhvr behaviors/GenericParticleFade.bhvr
			Part :FlamingSword_Blade_Core.part
			Part :FlamingSword_Blade_Core_Chunks.part
			Part :FlamingSword_Flame_Bits.part

		End

		Event
			At 	SwordAnchor
			AnimPiv	Head
			Type 	Local
			POther 	HiltAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :FlamingSword_Blade_Highlights.part
			Part :FlamingSword_Blade_Core2.part
			Part :FlamingSword_Blade_Trail.part

		End

		Event
			At 	SwordAnchor
			AnimPiv	Hips
			Type 	Local
			BhvrOverride
				PositionOffset	0.0 0.0 0.3
			End
			Part 	:FlamingSword_Guard_Core_Solid_Inv.part
			Part 	:FlamingSword_Guard_Core_Inv.part
			Part 	:FlamingSword_Guard_Highlights_Inv.part
			Part 	:FlamingSword_Guard_Core_Solid.part
			Part 	:FlamingSword_Guard_Core.part
			Part 	:FlamingSword_Guard_Highlights.part
		End

		Event
			At 	SwordAnchor
			AnimPiv	Neck
			Type 	Local
			POther 	HiltAnchor
			BhvrOverride
				PositionOffset	0.0 0.0 -0.3
			End
			Part 	:FlamingSword_Grip_Highlights.part
			Part	:FlamingSword_Grip_Core.part
		End
	End

###########################################################

End
