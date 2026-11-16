#########################################################
##	
##

FxInfo
LifeSpan  600

##################################

Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		bhvroverride
			positionoffset  0 0 .5
			scale		.01 .01 1
			HueShift	275
		end
#		part	:P_Teleportation_Spark.part
		pmagnet	mag
	End
End
Condition
	On Time
	Time 0
	Event
		ename	spinner1
		Type	Local
		At	prime 
		bhvroverride
			spinjitter	0 .1 0
			spin		0 .2 0
			HueShift	275
		end
	End
	Event	
		ename	mag1
		Type	Local
		At	spinner1 
		bhvroverride
			positionoffset .5 0 0
			HueShift	275
		end
		part	:P_Teleportation_Spark.part
		part	:P_Teleportation_Spark2.part
		part	:P_Teleportation_Sparklinger.part
		part	:P_Teleportation_SparkKickStart.part
	End
	Event	
		ename	mag2
		Type	Local
		At	spinner1
		bhvroverride
			positionoffset -.5 0 0
			HueShift	275
		end
		part	:P_Teleportation_Spark.part
		part	:P_Teleportation_Spark2.part
		part	:P_Teleportation_Sparklinger.part
		part	:P_Teleportation_SparkKickStart.part
	End
End

end
