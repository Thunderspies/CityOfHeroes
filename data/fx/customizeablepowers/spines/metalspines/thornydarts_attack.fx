#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	13

	Event

		Type	Local
		At	Origin
		Sound Darttoss 70 70 0.7
	End
End

Condition
	On	Time
	Time	15

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
		Geom	MetalSpine_SmallProjectile01
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