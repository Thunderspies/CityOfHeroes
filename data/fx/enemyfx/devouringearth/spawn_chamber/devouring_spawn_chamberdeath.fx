#########################################################
##	Template

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

	Event
		EName	Offset_01
		Type	Local
		At	origin
		Bhvr	:Offset_01.bhvr

	End

	Event
		EName	Body_01
		Type	Local
		At	Offset_01
		Part	:Body_Glow_DeathDark.part
		Part	:Body_Glow_Death.part
		Part	:DeathRocks.part
		LifeSpan		20

	End
	
	Event
		EName	Body_01
		Type	Local
		At	Offset_01
		Part	:DeathShine.part
		Part	:DeathShineBlack.part
		LifeSpan		5

	End

	Event
		EName	Inner_Beam_01
		Type	Local
		At	Offset_01
		#Part	:Inner_Beam_Summon.part
		LifeSpan		20
	End

End

End
