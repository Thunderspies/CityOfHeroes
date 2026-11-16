#########################################################
##	template

FxInfo

Flags    InheritAlpha


#Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
##	Event
##		Type Local
##		At origin
##		Sound 	Materialize1 500.0 100.0 .5
##	End
#		
#End

#########################################################
################## WARP OUT FLASH #######################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hook
		Type	local
		At	Origin
	
		BhvrOverride
			PositionOffset		36 -20 0
		End

		Part1	:TeleporterStreaks.part
		Part	:TeleporterStreaksLarger.part
		Part	:TeleporterStreaksWidest.part
		Part2	:RiktiTeleporterSpark.part
		Part	:RiktiTeleporterSparksDown.part
	End

End

End		