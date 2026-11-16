#############################################################
## Impale_Attack.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Origin
		Sound thorntoss3 70 70 0.79
	End
End

Condition
	On	Time
	Time	42


	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	CustomizeablePowers\Spines\Thorns\SpineProjectile.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailBlackBitsA.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailLightGreenBitsA.part
		Part	CustomizeablePowers\Spines\Thorns\QuillTrailLightBlackBitsA.part
		Geom	Tintable_SpineProjectile2
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