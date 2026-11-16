#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Origin
		Sound Quills5 70 70 0.7
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\ImpaleProjectile_Grow.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	SlateSpines_LargeProjectile01
		Lifespan 32
	End
End

Condition
	On	Time
	Time	42

	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	CustomizeablePowers\Spines\Spines\Quillprojectile1.bhvr
		BhvrOverride
			Spin			0 0 1
			TintGeom		1
		End
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailGreenBitsA.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailBlackBitsA.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightGreenBitsA.part
		Part	CustomizeablePowers\Spines\Spines\QuillTrailLightBlackBitsA.part
		Geom	SlateSpines_LargeProjectile01
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