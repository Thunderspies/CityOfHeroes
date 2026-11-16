#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

## WINGS ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	WingAnim
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_BURNED_Male.bhvr
		Anim	FX_wings_BURNED
	End
End

## WING FX ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	Hips
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandL
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End


	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandR
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_R
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_L
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 1 -2.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeAura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 1 -2.5
		End
		Part	AnimatedCharacterParts/BurntWingSmokeaura.part
	End
End

## LICKS OF FLAME ###########################################################

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	LarmL
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	LarmR
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmL
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmR
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandL
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandR
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_R
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_L
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05
	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR
		BhvrOverride
			PositionOffset	0 1 -2.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL
		BhvrOverride
			PositionOffset	0 1 -2.5
		End
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part
		LifeSpan	5
	End
End

#############################################################

End