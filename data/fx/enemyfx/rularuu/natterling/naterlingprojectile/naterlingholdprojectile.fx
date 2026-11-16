#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200


Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound Natterlingthrow 80 80 .7
	End
End


Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/NatterlingHoldProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:CrystalTrail.part
		Part	:CrystalTrail2.part
		Part	:FleshTrail.part
		Part	:FleshTrail2.part
		Part	:FleshTrailBlack.part
		
		Geom	NatterlingProjectile
		
	End

End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime1
		Type	Start 
		At	WepR
		Bhvr	behaviors/NatterlingHoldProjectile.bhvr
		Magnet	Target
		LookAt	Target

		Geom	NatterlingProjectileCrystals
		
	End

End

Condition
	On	PrimeHit
	
	Event
		Type	local 
		At	T_waist
		
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodePurple.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeBlue.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplode.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/streakSparks.part
		sound	crystalhit 60 50 .6
		LifeSpan	7

	End
			
	Event
		Ename	Prime
		Type	Destroy
	
	End

End

Condition
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End

	
End

End	


			