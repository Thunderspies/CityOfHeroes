#############################################################
## Cond Control - Chillblain
#############################################################

FxInfo

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Rockup
		Type	Local
		At	Origin
		Bhvr	Behaviors/Braz/Geo_GrowFromBelow.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			Scale 		0.3 0.6 0.1
			StartJitter	5 0 5
			Alpha 		255
			PyrRotate 	0 180 0
			FadeinLength	0
			FadeOutLength	0
		End
		Geom	Cliff_Chunk_01
		Geom	Cliff_Chunk_02
		Lifespan 60
	End

	Event
		EName 	dirtup
		Type	Local
		At	Origin
	#	Part	:Dirt_Stream_Down.part
	#	Part	:Dirt_Lingering.part
	End



End

#############################################################

End