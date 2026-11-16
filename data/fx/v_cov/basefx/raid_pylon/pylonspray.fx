#########################################################
##	forceBubble
########################################################
FxInfo

lifespan 500

Flags    InheritAlpha

#################################################################################

##	Needsound: this is a black and red power disk below the caster



Condition
	On	Time
	Time	0

	Event
		Type	start
		At	root
		part	:Runeshadow.part
		bhvroverride
			PositionOffset		0 .110 0
			scale 1 1 1
		end
	End
	Event
		EName	Base
		Type	start
		At	root
		bhvroverride
			PositionOffset		0 1 0
			scale .8 .8 .8
		end
	End


	Event
		EName	tr3
		Type	local
		At	Base
		part	:Rune_Drips4.part
#		bhvr	:Fadey.bhvr
#		Geom	_raidAnchor_Runes3__runespin
		bhvroverride
		FadeInLength		20
			scale			1 1 1
			PositionOffset		0 -3 0
		end
	End
	Event	
		Ename	SprayBase1
		Type	local
		At	base
		bhvr	:spraygrow.bhvr
		bhvroverride
			Spin			0.0 0.10 0.0
			PositionOffset		0 -4.5 0
			EndScale		.8 .8 .8
		end
	End

	Event
		Type	local
		At	SprayBase1
		part	:Ring_Spray.part
		part	:Ring_Spray_Spark.part
		bhvroverride
			PositionOffset		0 7 0
		end
		pother SprayBase1
	End

	

	Event	
		Ename	SprayBase2
		Type	local
		At	base
		bhvr	:spraygrow.bhvr
		bhvroverride
			Spin			0.0 -0.10 0.0
			PositionOffset		0 -6.5 0
			EndScale		.8 .8 .8
		end
	End

	Event
		Type	local
		At	SprayBase2
		part	:Ring_Spray.part
		part	:Ring_Spray_Spark.part
		bhvroverride
			PositionOffset		0 10 0
		end
		pother SprayBase2
	End

End 