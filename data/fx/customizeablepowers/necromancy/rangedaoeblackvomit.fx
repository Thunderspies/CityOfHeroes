#############################################################
## RangedAOEBlackVomit.fx
#############################################################

FxInfo
LifeSpan	300

#############################################################

Condition

	On 	Time
	Time 	20

	Event
		Type	Local
		At	head
		Sound blackvomitAOE3 100.0 100.0 .77
	End

	On 	Time
	Time 	27



	Event
		EName	P
		Type	Local
		At	head

		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr
	End

	Event
		EName	Prime
		Type	start
		At	P
		Altpiv   1

		BHVR	Behaviors/VomitProjectile.bhvr
		Part	:CoreBubbles.part
		Part	:Core.part
		Part1	:PoisonSpit.part
		Part2	:PoisonSpitBubbles.part
		Part3	:PoisonSpecksSpit.part
		Part3	:PoisonSpecks.part
		Part1	:PoisonBallz.part


		magnet	t_hips
	End
End


Condition
	On 	PrimeHit

	Event
		EName	Prime
		Type	Destroy

	End
End

#############################################################

End