#########################################################
##	FireAura
FxInfo

################################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	root
		Sound arcoflame 80 80 .55
	End

	Event
		Type	start
		At	root
		Sound mudpots_loop 80 80 .45
	End

	Event
		Ename	SpinnerNode
		Type	start
		At	root

		BhvrOverride
			
			Spin	0 .05 0
		End
		geom	FX_Iceweapon
	End

End

Condition

#	Event
#		EName	Pivotx
#		Type	start
#		At		root
#		BHVR	:QuickSandMudSplatBase.bhvr
#		Splat	quicksand.tga	#mud.tga
#		LifeSpan	120
#	End


	Event
		EName	Pivotx
		Type	local
		At	SpinnerNode
		BHVR	:QuickSandMudSplatSwirl.bhvr
		Splat	quicksandSwirl.tga
		LifeSpan	120
	End

End


#Condition
#	On	cycle
#	Time	45
#
#	Event
#		EName	Pivotx
#		Type	start
#		At		root
#		BHVR	:QuickSandMudSplatBase.bhvr
#		Splat	quicksand.tga	#mud.tga
#		LifeSpan	120
#	End
#End

Condition
	On	cycle
	Time	40

	Event
		EName	Pivotx
		Type	local
		At	SpinnerNode
		BHVR	:QuickSandMudSplatSwirl.bhvr
		Splat	quicksandSwirl.tga
		LifeSpan	120
	End
#End

End				