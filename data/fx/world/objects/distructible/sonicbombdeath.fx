#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

######################################################################


Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Root
		Sound grexplo4 60 60 .66

	End

End



Condition
	On 	cycle
	Time 	7
	Chance	1

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:splat6.bhvr
		
		splat	generic_Ring.tga

		#Part1	:GasFlat.part
		#Part	:TearGas01.part
		#Part1	:TearGasFlat.part
		Sound miasma_loop 50 50 .25

		LifeSpan	20
	End

	Event
		EName 	StoneDustRingHit2
		Type	start
		At	Root
		Bhvr	:splat6dark.bhvr
		
		splat	ThinRing.tga

		LifeSpan	10
	End

End


Condition
	On 	triggerbits
	Triggerbits	death
	
################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################ deathFx because he'll be dead before the fx happens.)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanishSlow2.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	StoneDustRingHit
		Type	Destroy
	End

	Event
		EName 	StoneDustRingHit2
		Type	Destroy
	End

	Event
		EName 	Explosion
		Type	Local
		At	Origin
		ChildFx	Explosions/MetalExplosion/MetalExplosionChild.fx
	End

	Event
		Type	start
		At	Root
		Bhvr	:splat6.bhvr
		
		splat	generic_Ring.tga

		#Part1	:GasFlat.part
		#Part	:TearGas01.part
		#Part1	:TearGasFlat.part
		Sound miasma_loop 50 50 .25

		LifeSpan	20
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:splat6dark.bhvr
		
		splat	ThinRing.tga

		LifeSpan	10
	End

End

End
##################################

