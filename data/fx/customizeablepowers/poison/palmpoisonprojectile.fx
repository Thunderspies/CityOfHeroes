#############################################################
## PalmPoisonProjectile.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		EName	Prime
		Type	start
		At	WepL
		Bhvr	CustomizeablePowers\Poison\Quillprojectile1.bhvr
		Part	CustomizeablePowers\Poison\QuillTrailBlack.part
		Part	CustomizeablePowers\Poison\QuillTrail.part
		Part	CustomizeablePowers\Poison\QuillTrailLight.part
		Part	CustomizeablePowers\Poison\QuillTrailStreak.part
		Part	CustomizeablePowers\Poison\QuillBallzTrail.part
		Part	CustomizeablePowers\Poison\QuillBallzTrailBlack.part
		Sound PoisonPalmBlast 90 90 0.84
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End