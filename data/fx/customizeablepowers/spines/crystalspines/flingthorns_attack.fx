#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	WepR
		Sound thornytoss2 70 70 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	13

	Event
		Ename	HandNode
		Type	start
		At	BendMystic
		geom	Centerdummy
		magnet	target
		lookat	target
	End

	Event
		Ename	projectileTarget
		Type	local
		At	HandNode
		Altpiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset01
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset02
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset03
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset04
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset05
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End

	Event
		Ename	TargetOffset06
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	CustomizeablePowers\Spines\Thorns\offset.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		Magnet	target
		Lookat	target
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	projectileTarget
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset05
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset04
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset03
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset02
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	Prime2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset01
		Lookat	HandNode
		LifeSpan 17
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	p
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset06
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset05
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset04
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset03
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset02
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Spines\Thorns\projectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	TargetOffset01
		Lookat	HandNode
		LifeSpan 17
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Type	Local
		At	chest
		Sound Quills6 70 70 0.7
		Lifespan 1
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End