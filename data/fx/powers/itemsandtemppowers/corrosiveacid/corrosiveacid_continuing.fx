#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 340

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake01Subtle.bhvr

		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	EnemyFX\Praetorian\Warworks\Toxic_Splash_Initial.part
		Part	EnemyFX\Praetorian\Warworks\Toxic_Splash_Initial_Lingering.part

		Lifespan 4
	End
End


Condition
	On 	Time
	Time 	2



End

#############################################################

End