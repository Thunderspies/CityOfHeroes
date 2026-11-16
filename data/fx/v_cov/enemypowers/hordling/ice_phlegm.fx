#########################################################
##	IceBolt
############################
FxInfo

LifeSpan	200

##################################


Condition
	On 	Time
	Time 	7



	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivot01
		bhvr	behaviors/scale1a.bhvr	
		Sound Breathattack 80.0 80.0 .8
		
		Lifespan	15
	End

	Event
		EName 	cone
		Type	Local 
		At	Spike
		Altpiv	1

		Part1	POWERS\ColdControl\FrostBreathIce.part
		Part2	POWERS\ColdControl\FrostBreathSnow.part
		Part1	POWERS\ColdControl\FrostBreathIce.part
		Part2	POWERS\ColdControl\FrostBreathSnow.part
		
	End
	
End

Condition
	On 	Time
	Time 	15.2

	Event
		EName 	Prime
		Type	Start 
		At	Head
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part1	POWERS\ColdControl\icebolttail3.part
#		Part2	POWERS\ColdControl\icebolttail2.part
#		Part3	POWERS\ColdControl\IceBoltSnowtrail2.part
		Part4	POWERS\ColdControl\IceBoltMistTrail2.part
		Part5	POWERS\ColdControl\icebolttail.part
		
	End
	
End
###########################
Condition
	On 	Time
	Time 	0



	Event
		EName 	loogey
		Type	Local 
		At	Head

		Sound Ice9 100.0 100.0 .75

	End


End


##############################################################


Condition
	On	PrimeHit
		
	Event
		Ename	All
		Type	Destroy
	End

	Event	
		Ename   IceShards
		Type	Posit
		At	target
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part2	POWERS\ColdControl\ChillTouchSnow.part
		Part3   POWERS\ColdControl\ChillingTouchFlash.part

		LifeSpan	20

	End

End


End				