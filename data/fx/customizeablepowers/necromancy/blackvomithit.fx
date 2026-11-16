#############################################################
## BlackVomitHit.fx
#############################################################

FxInfo
LifeSpan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	start
		At	chest

		Part	:HitBlackVomitSpit.part
		Part	:BLKHitBlackVomitSpit.part

		LifeSpan 10

	End

	Event

		Type	start
		At	chest

		Part	:HitBlackVomitBallz.part
		Part	:HitBlackVomitDrips.part
		Part	:HitBlackVomitDripsLITE.part
		Part	:HitBlackVomitChunks.part

		LifeSpan 20

	End

End

#############################################################

End