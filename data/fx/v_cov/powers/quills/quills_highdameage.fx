#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	WepR
		Sound quills1 50 50 .8
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	WepR
		Sound thornbarrage2 80 80 .96
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors\SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailBlackBitsA.part
		Part	powers\PoisonControl\QuillTrailLightGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailLightBlackBitsA.part
	End
End

Condition
	On	Time
	Time	32

	Event
		EName	Prime1
		Type	start
		At	WepL
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors\SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailBlackBitsA.part
		Part	powers\PoisonControl\QuillTrailLightGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailLightBlackBitsA.part
	End
End

Condition
	On	Time
	Time	35
	Event
		EName	Prime2
		Type	start
		At	WepR
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors\SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailBlackBitsA.part
		Part	powers\PoisonControl\QuillTrailLightGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailLightBlackBitsA.part
	End
End

Condition
	On	Time
	Time	42
	Event
		EName	Prime3
		Type	start
		At	WepL
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors\SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailBlackBitsA.part
		Part	powers\PoisonControl\QuillTrailLightGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailLightBlackBitsA.part
	End
End

Condition
	On	Time
	Time	49

	Event
		EName	Prime4
		Type	start
		At	WepL
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors\SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailBlackBitsA.part
		Part	powers\PoisonControl\QuillTrailLightGreenBitsA.part
		Part	powers\PoisonControl\QuillTrailLightBlackBitsA.part
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

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End
End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
	End

	Event
		EName 	P4
		Type	Destroy
	End
End

#############################################################

End