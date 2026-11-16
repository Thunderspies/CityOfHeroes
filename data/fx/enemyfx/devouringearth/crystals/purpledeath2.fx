#########################################################
##	
#########################################################

FxInfo


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
		Bhvr	behaviors/vanishSlow.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
#################### End do this

	Event
		EName	Gas
		Type	Local
		At	chest
		Bhvr	Behaviors/GenericParticleFadeoffset.bhvr
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
		Part	:PurpleCrystalScatter01.part		
		Part	:PurpleCrystalScatter02.part
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
		Part	:PurpleCrystalScatter01.part		
		Part	:PurpleCrystalScatter02.part

		#Sound	poisonhit2 50 30 .7
		LifeSpan	30
	End
End

End