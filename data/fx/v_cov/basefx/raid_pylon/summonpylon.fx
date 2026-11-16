#########################################################
##	forceBubble
########################################################
FxInfo



Flags    InheritAlpha

#################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	origin
		Sound PylonSummon3 100 100 .7

	End
End

Condition   
	On	Time
	Time	0

	Event
		EName	Base
		Type	start
		At	origin
		bhvr	:shadowgrow.bhvr
		part	:Runeshadow.part
		bhvroverride
			Spin			0.0 0.03 0.0
			PositionOffset		0 -.11 0
		end
	End
	Event
		EName	BaseL
		Type	start
		At	origin
		bhvroverride
			Spin			0.0 0.03 0.0
			PositionOffset		0 0 0
		end
	End
	Event
		EName	BaseR
		Type	start
		At	origin
		bhvroverride
			Spin			0.0 -0.03 0.0
			PositionOffset		0 0 0
		end
	End
end
######### Disks

Condition   
	On	Time
	Time	0

	Event
		EName	tr3
		Type	local
		At	BaseL
		part	:Rune_Drips1.part
		part	:groundRune3.part
#		bhvr	:Fadey.bhvr
#		Geom	_raidAnchor_Runes3__runespin
		bhvroverride
		FadeInLength		20
			scale			1 1 1
			PositionOffset		0 0 0
		end
	End

end
Condition   
	On	Time
	Time	30

	Event
		EName	tr2
		Type	local
		At	BaseR
		part	:Rune_Drips2.part
		part	:groundRune2.part
#		bhvr	:Fadey.bhvr
#		Geom	_raidAnchor_Runes2__runespin
		bhvroverride
		FadeInLength		20
			scale			1 1 1
			PositionOffset		0 0 0
		end
	End
End
Condition   
	On	Time
	Time	60
###
	Event
		EName	tr1
		Type	local
		At	BaseL
		part	:Rune_Drips3.part
		part	:groundRune1.part
#		bhvr	:Fadey.bhvr
#		Geom	_raidAnchor_Runes1__runespin
		bhvroverride
		FadeInLength		20
			scale			1 1 1
			PositionOffset		0 0 0
		end
	End
End

#### spraying rings

Condition
	On	Time
	Time	50

	Event	
		Ename	SprayBase1
		Type	startpositonly
		At	base
		bhvr	:spraygrow.bhvr
		bhvroverride
			Spin			0.0 0.10 0.0
			PositionOffset		0 -4.5 0
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
		Type	startpositonly
		At	base
		bhvr	:spraygrow.bhvr
		bhvroverride
			Spin			0.0 -0.10 0.0
			PositionOffset		0 -6.5 0
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

######### runes

Condition   
	On	cycle
	Time	8
	Event
		Type	local
		At	TR2
		childfx :SummonRunes.fx
		lifespan 70
	end

end

Condition   
	On	cycle
	Time	10
	Event
		Type	local
		At	TR1
		childfx :SummonRunesBig.fx
		lifespan 70
	end

end
