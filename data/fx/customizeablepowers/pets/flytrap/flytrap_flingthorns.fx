#############################################################
## Quills_cone.fx
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
	Time	0

	Event
		Type	Start
		At	Root
		ChildFX CustomizeablePowers\Pets\FlyTrap\Child_ThornGrow.fx
		lifespan 25
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	HandNode
		Type	Start
		At	C_Hind_LlegL
		Geom	Centerdummy
		magnet	target
		lookat	target
	End

	Event
		Ename	projectileTarget
		Type	local
		At	HandNode
		Altpiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	1
		Type	local
		At	HandNode
		Altpiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	2
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	3
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	4
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	5
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	6
		Type	local
		At	HandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName 	prime
		Type	start
		At	C_Hind_LlegL
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Magnet	target
		Lookat	target
	End

	Event
		EName 	p
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	projectileTarget
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	5
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	4
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	3
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	2
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	1
		Lookat	HandNode
		LifeSpan 17
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	p
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	6
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p5
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	5
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p4
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	4
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p3
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	3
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p2
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	2
		Lookat	HandNode
		LifeSpan 17
	End

	Event
		EName 	p1
		Type	start
		At	HandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	1
		Lookat	HandNode
		LifeSpan 17
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	RHandNode
		Type	start
		At	C_Hind_LlegR
		geom	Centerdummy
		magnet	target
		lookAt	Rtarget
	End

	Event
		Ename	projectileTarget
		Type	local
		At	RHandNode
		Altpiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	1
		Type	local
		At	RHandNode
		Altpiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	2
		Type	local
		At	RHandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	3
		Type	local
		At	RHandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	4
		Type	local
		At	RHandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	5
		Type	local
		At	RHandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End

	Event
		Ename	6
		Type	local
		At	RHandNode
		AltPiv	1
		BHVR	V_COV\Powers\PlantControl\Flytrap\offset.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	21


	Event
		EName 	prime2
		Type	start
		At	C_Hind_LlegR
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Magnet	target
		LookAt	target
	End

	Event
		EName 	Rp
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	projectileTarget
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp5
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	5
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp4
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	4
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp3
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	3
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp2
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	2
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp1
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	1
		LookAt	RHandNode
		LifeSpan 17
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Rp
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	6
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp5
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	5
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp4
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	4
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp3
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	3
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp2
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	2
		LookAt	RHandNode
		LifeSpan 17
	End

	Event
		EName 	Rp1
		Type	start
		At	RHandNode
		altpiv	1
		Bhvr	CustomizeablePowers\Pets\FlyTrap\projectile.bhvr
		Geom	Tintable_SmallSpineProjectile01
		Geom	Tintable_SmallSpineProjectile02
		Geom	Tintable_SmallSpineProjectile03
		Geom	Tintable_SmallSpineProjectile04
		Magnet	1
		LookAt	RHandNode
		LifeSpan 17
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

#############################################################

End