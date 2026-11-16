#############################################################
## CorruptionAttack.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	Prime
		At 	WepR
		Type	Start
		Bhvr	behaviors\BeanBagProjectile.bhvr
		BhvrOverride
			TrackRate		3.0
		End
		Part	:Corrupution_Projectile.part
		Part	:Corrupution_Projectile_Highlight.part
		Magnet	Target
		Lookat	Target
	End
End


Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End