#############################################################
## SparksBody.fx
#############################################################

FxInfo


#############################################################

Condition
	Event
		Type	Local
		At	Origin
				BhvrOverride
			PositionOffset 0 -6 0
		End
		# Part flash.part
		SoundNoRepeat 3
		Sound Electric_Spark_01 25 25 .1
		Sound Electric_Spark_02 25 25 .05
		Sound Electric_Spark_03 25 25 .15
		Sound Electric_Spark_04 25 25 .15
		Sound Electric_Spark_05 25 25 .25
		Sound Electric_Spark_06 25 25 .1
		lifespan 1
	End
End
