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
	On	Time
	Time	42

	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	behaviors\Quillprojectile1.bhvr
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part1	:QuillTrailGreenBitsA.part
		Part2	:QuillTrailBlackBitsA.part
		Part4	:QuillTrailLightGreenBitsA.part
		Part5	:QuillTrailLightBlackBitsA.part
		Geom	SpineProjectile
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