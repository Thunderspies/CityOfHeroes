#############################################################
## GrenadeThrow_Smoke.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound grtoss1 60 60 .7
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	wepR
		geom	grenade
		bhvr	CustomizeablePowers\Gadgets\grenadeFade.bhvr
		lifespan 22
	End
End

#############################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	CustomizeablePowers\Gadgets\MorterLobbprojectile2.bhvr
		geom	grenade
		Part1	CustomizeablePowers\Gadgets\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Smoke
		Type	Posit
		At	T_Root
		Part1	CustomizeablePowers\Gadgets\DynamiteExplodeSmoke1.part
		Part1	CustomizeablePowers\Gadgets\DynamiteExplodeSmoke2.part
		Part	:SmokeScreen.part
		Lifespan 60
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	CustomizeablePowers\Gadgets\FireExplosionFlash.part
		Part2	CustomizeablePowers\Gadgets\DynamiteExplodeSparks.part
		Lifespan 5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	10
		End
		Lifespan	1
	End

	Event
		Ename 	Prime
		Type	Destroy
	End
End

##################################

Condition
	On 	Time
	Time	34

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	PrimeHit



End

#############################################################

End