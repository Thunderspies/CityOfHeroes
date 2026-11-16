#########################################################
##	Spit_Ball
##

#########################################################

FxInfo

LifeSpan	200


#########################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	Spike
		Type	Local
		At	Head
		
		Geom	LocalPivotz
#		bhvr	behaviors/scale1a.bhvr	
		SOund	poisonprojectile 80 30 .88
	End


	Event
		EName	Prime
		Type	start
		At	Spike
		 Altpiv	1
	
		Bhvr	behaviors/FX_SnakeMen_Spit.bhvr
		Magnet	Target
		LookAt	Target
#		Part1	:SM_SpitTrail2.part
		Part2	:SM_SpitPoisonHead.part
		Part2	:SM_SpitPoisonHeadSM.part
		PMagnet	Prime
		
	End



	Event
		EName	Trails
		Type	Start
		At	Prime
#		 Altpiv	1

		Bhvr	behaviors/FX_SnakeMen_Spit.bhvr
		Magnet	Target
		LookAt	Target
#		Part1	:SM_SpitTrail.part
		Part2	:SM_SpitTrailSpecks.part
		Part3	:SM_SpitBubblesTrail.part
		Part5	:SM_SpitBallzTrail.part
		Lifespan 20
		PMagnet	Prime
	End

	Event
		EName	Burst
		Type	Start
		At	Spike
		Altpiv	1

		Bhvr	behaviors/FX_SnakeMen_SpitBurst.bhvr
		Part1	:SM_SpitTrail.part
		Lifespan 3
	End


	
End

Condition
	On 	PrimeHit

	Event
		EName	Burst
		Type	Start
		At	Prime

		ChildFX	:SnakeMen_SpitMiss.fx
	End

	Event
		EName 	Prime
		Type	Destroy
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Trails
		Type	Destroy
		
	End

End

End				