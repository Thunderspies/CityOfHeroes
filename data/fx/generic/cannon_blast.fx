#########################################################
##	
## 

FxInfo

############################################

LifeSpan 30


Condition
	On	Time
	Time	0

#	Event
#		EName 	Blast
#		Type	Start
#		At	Mystic
#		Bhvr	:Offset_Cannon_blast.bhvr
#		Geom	Cannon_blast
#		LifeSpan 2
#
#	End


	Event
		EName 	flash
		Type	Start
		At	Mystic

		Part	WEAPONFX/FireExplosionFlash.part
		Lifespan 5

	End



	Event
		EName 	sound
		Type	Start
		At	WepR
		Sound Musket 100.0 100.0 .9

	End
End




