
#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
End

Input  
	InpName	Origin
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
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
#################### End do this
	
	
	Event
		EName   RadiusFxScalex
		Type	local
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScalex
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	powers/firecontrol/FireShieldCloud01B.part
		Part2	powers/firecontrol/FireShieldCloud02B.part
		Part4	Explosions\FireExplosion\FireShieldSparksWhite.part
		
		LifeSpan	30
		
	End

	Event
		
		Type	local
		At	root
		
		ChildFx	Explosions\FireExplosion\FireExplosionMedium.fx	
		LifeSpan	100
	End

	Event

		EName	Gas
		Type	Local
		At	chest

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
		Part	:PinkCrystalScatter01.part		
		Part	:PinkCrystalScatter02.part
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
		Part	:PinkCrystalScatter01.part		
		Part	:PinkCrystalScatter02.part

		#Sound	poisonhit2 50 30 .7
		LifeSpan	30
		
	End
	
End

End
