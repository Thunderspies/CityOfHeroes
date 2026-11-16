#############################################################
## BaseForceBubble.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	root
		BhvrOverride
			PositionOffset	0.0 3.0 0.0
			Scale		1.1 1.1 1.1
			Spin		0.02 0.02 0.02
			StartColor	255 32 16
			Alpha		175
		End
		Geom	FifthColumnForceField
		Part	:Bubble_Haze.part
	End

	Event
		Ename	Upper_Offset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
	End

	Event
		Type	Local
		At	Root
		Part	:Embers_Warping.part
		PMagnet	Upper_Offset
	End

	Event
		EName	lightning
		Type	local
		At	root
		Part	:Ground_Haze.part
		Part	:Ground_Perimeter_Electricity.part
		Part	:Ground_Perimeter.part
		Part2	:ChainLightningArchs3.part
		Part3	:ChainLightningArchs2.part
		Part4	:ChainLightningArchs1.part
		Part5	:ChainLightningArchs.part
		##Sound	ChainLight2_loop 80 70 .5
	End
End

#############################################################

End