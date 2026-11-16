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
		Bhvr	behaviors\Quillprojectile1.bhvr
		Part	:QuillTrailBlack.part
		Part	:QuillTrail.part
		Part	:QuillTrailLight.part
		Part	:QuillTrailStreak.part
		Part	:QuillBallzTrail.part
		Part	:QuillBallzTrailBlack.part
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