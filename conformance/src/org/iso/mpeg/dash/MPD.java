//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-792 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2012.11.30 at 09:10:51 AM MEZ 
//


package org.iso.mpeg.dash;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAnyAttribute;
import javax.xml.bind.annotation.XmlAnyElement;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;
import javax.xml.datatype.Duration;
import javax.xml.datatype.XMLGregorianCalendar;
import javax.xml.namespace.QName;
import org.w3c.dom.Element;


/**
 * <p>Java class for MPDtype complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="MPDtype">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="ProgramInformation" type="{urn:mpeg:dash:schema:mpd:2011}ProgramInformationType" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="BaseURL" type="{urn:mpeg:dash:schema:mpd:2011}BaseURLType" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="Location" type="{http://www.w3.org/2001/XMLSchema}anyURI" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="Period" type="{urn:mpeg:dash:schema:mpd:2011}PeriodType" maxOccurs="unbounded"/>
 *         &lt;element name="Metrics" type="{urn:mpeg:dash:schema:mpd:2011}MetricsType" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;any processContents='lax' namespace='##other' maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;/sequence>
 *       &lt;attribute name="id" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="profiles" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="type" type="{urn:mpeg:dash:schema:mpd:2011}PresentationType" default="static" />
 *       &lt;attribute name="availabilityStartTime" type="{http://www.w3.org/2001/XMLSchema}dateTime" />
 *       &lt;attribute name="availabilityEndTime" type="{http://www.w3.org/2001/XMLSchema}dateTime" />
 *       &lt;attribute name="publishTime" type="{http://www.w3.org/2001/XMLSchema}dateTime" />
 *       &lt;attribute name="mediaPresentationDuration" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="minimumUpdatePeriod" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="minBufferTime" use="required" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="timeShiftBufferDepth" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="suggestedPresentationDelay" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="maxSegmentDuration" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;attribute name="maxSubsegmentDuration" type="{http://www.w3.org/2001/XMLSchema}duration" />
 *       &lt;anyAttribute processContents='lax' namespace='##other'/>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "MPDtype", propOrder = {
    "programInformations",
    "baseURLs",
    "locations",
    "periods",
    "metrics",
    "anies"
})
@XmlRootElement(name = "MPD")
public class MPD {

    @XmlElement(name = "ProgramInformation")
    protected List<ProgramInformationType> programInformations;
    @XmlElement(name = "BaseURL")
    protected List<BaseURLType> baseURLs;
    @XmlElement(name = "Location")
    @XmlSchemaType(name = "anyURI")
    protected List<String> locations;
    @XmlElement(name = "Period", required = true)
    protected List<PeriodType> periods;
    @XmlElement(name = "Metrics")
    protected List<MetricsType> metrics;
    @XmlAnyElement
    protected List<Element> anies;
    @XmlAttribute
    protected String id;
    @XmlAttribute(required = true)
    protected String profiles;
    @XmlAttribute
    protected PresentationType type;
    @XmlAttribute
    @XmlSchemaType(name = "dateTime")
    protected XMLGregorianCalendar availabilityStartTime;
    @XmlAttribute
    @XmlSchemaType(name = "dateTime")
    protected XMLGregorianCalendar availabilityEndTime;
    @XmlAttribute
    @XmlSchemaType(name = "dateTime")
    protected XMLGregorianCalendar publishTime;
    @XmlAttribute
    protected Duration mediaPresentationDuration;
    @XmlAttribute
    protected Duration minimumUpdatePeriod;
    @XmlAttribute(required = true)
    protected Duration minBufferTime;
    @XmlAttribute
    protected Duration timeShiftBufferDepth;
    @XmlAttribute
    protected Duration suggestedPresentationDelay;
    @XmlAttribute
    protected Duration maxSegmentDuration;
    @XmlAttribute
    protected Duration maxSubsegmentDuration;
    @XmlAnyAttribute
    private Map<QName, String> otherAttributes = new HashMap<QName, String>();

    /**
     * Gets the value of the programInformations property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the programInformations property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getProgramInformations().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ProgramInformationType }
     * 
     * 
     */
    public List<ProgramInformationType> getProgramInformations() {
        if (programInformations == null) {
            programInformations = new ArrayList<ProgramInformationType>();
        }
        return this.programInformations;
    }

    /**
     * Gets the value of the baseURLs property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the baseURLs property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getBaseURLs().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link BaseURLType }
     * 
     * 
     */
    public List<BaseURLType> getBaseURLs() {
        if (baseURLs == null) {
            baseURLs = new ArrayList<BaseURLType>();
        }
        return this.baseURLs;
    }

    /**
     * Gets the value of the locations property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the locations property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getLocations().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link String }
     * 
     * 
     */
    public List<String> getLocations() {
        if (locations == null) {
            locations = new ArrayList<String>();
        }
        return this.locations;
    }

    /**
     * Gets the value of the periods property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the periods property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getPeriods().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link PeriodType }
     * 
     * 
     */
    public List<PeriodType> getPeriods() {
        if (periods == null) {
            periods = new ArrayList<PeriodType>();
        }
        return this.periods;
    }

    /**
     * Gets the value of the metrics property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the metrics property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getMetrics().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link MetricsType }
     * 
     * 
     */
    public List<MetricsType> getMetrics() {
        if (metrics == null) {
            metrics = new ArrayList<MetricsType>();
        }
        return this.metrics;
    }

    /**
     * Gets the value of the anies property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the anies property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAnies().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Element }
     * 
     * 
     */
    public List<Element> getAnies() {
        if (anies == null) {
            anies = new ArrayList<Element>();
        }
        return this.anies;
    }

    /**
     * Gets the value of the id property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getId() {
        return id;
    }

    /**
     * Sets the value of the id property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setId(String value) {
        this.id = value;
    }

    /**
     * Gets the value of the profiles property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getProfiles() {
        return profiles;
    }

    /**
     * Sets the value of the profiles property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setProfiles(String value) {
        this.profiles = value;
    }

    /**
     * Gets the value of the type property.
     * 
     * @return
     *     possible object is
     *     {@link PresentationType }
     *     
     */
    public PresentationType getType() {
        if (type == null) {
            return PresentationType.STATIC;
        } else {
            return type;
        }
    }

    /**
     * Sets the value of the type property.
     * 
     * @param value
     *     allowed object is
     *     {@link PresentationType }
     *     
     */
    public void setType(PresentationType value) {
        this.type = value;
    }

    /**
     * Gets the value of the availabilityStartTime property.
     * 
     * @return
     *     possible object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public XMLGregorianCalendar getAvailabilityStartTime() {
        return availabilityStartTime;
    }

    /**
     * Sets the value of the availabilityStartTime property.
     * 
     * @param value
     *     allowed object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public void setAvailabilityStartTime(XMLGregorianCalendar value) {
        this.availabilityStartTime = value;
    }

    /**
     * Gets the value of the availabilityEndTime property.
     * 
     * @return
     *     possible object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public XMLGregorianCalendar getAvailabilityEndTime() {
        return availabilityEndTime;
    }

    /**
     * Sets the value of the availabilityEndTime property.
     * 
     * @param value
     *     allowed object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public void setAvailabilityEndTime(XMLGregorianCalendar value) {
        this.availabilityEndTime = value;
    }

    /**
     * Gets the value of the publishTime property.
     * 
     * @return
     *     possible object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public XMLGregorianCalendar getPublishTime() {
        return publishTime;
    }

    /**
     * Sets the value of the publishTime property.
     * 
     * @param value
     *     allowed object is
     *     {@link XMLGregorianCalendar }
     *     
     */
    public void setPublishTime(XMLGregorianCalendar value) {
        this.publishTime = value;
    }

    /**
     * Gets the value of the mediaPresentationDuration property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getMediaPresentationDuration() {
        return mediaPresentationDuration;
    }

    /**
     * Sets the value of the mediaPresentationDuration property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setMediaPresentationDuration(Duration value) {
        this.mediaPresentationDuration = value;
    }

    /**
     * Gets the value of the minimumUpdatePeriod property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getMinimumUpdatePeriod() {
        return minimumUpdatePeriod;
    }

    /**
     * Sets the value of the minimumUpdatePeriod property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setMinimumUpdatePeriod(Duration value) {
        this.minimumUpdatePeriod = value;
    }

    /**
     * Gets the value of the minBufferTime property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getMinBufferTime() {
        return minBufferTime;
    }

    /**
     * Sets the value of the minBufferTime property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setMinBufferTime(Duration value) {
        this.minBufferTime = value;
    }

    /**
     * Gets the value of the timeShiftBufferDepth property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getTimeShiftBufferDepth() {
        return timeShiftBufferDepth;
    }

    /**
     * Sets the value of the timeShiftBufferDepth property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setTimeShiftBufferDepth(Duration value) {
        this.timeShiftBufferDepth = value;
    }

    /**
     * Gets the value of the suggestedPresentationDelay property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getSuggestedPresentationDelay() {
        return suggestedPresentationDelay;
    }

    /**
     * Sets the value of the suggestedPresentationDelay property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setSuggestedPresentationDelay(Duration value) {
        this.suggestedPresentationDelay = value;
    }

    /**
     * Gets the value of the maxSegmentDuration property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getMaxSegmentDuration() {
        return maxSegmentDuration;
    }

    /**
     * Sets the value of the maxSegmentDuration property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setMaxSegmentDuration(Duration value) {
        this.maxSegmentDuration = value;
    }

    /**
     * Gets the value of the maxSubsegmentDuration property.
     * 
     * @return
     *     possible object is
     *     {@link Duration }
     *     
     */
    public Duration getMaxSubsegmentDuration() {
        return maxSubsegmentDuration;
    }

    /**
     * Sets the value of the maxSubsegmentDuration property.
     * 
     * @param value
     *     allowed object is
     *     {@link Duration }
     *     
     */
    public void setMaxSubsegmentDuration(Duration value) {
        this.maxSubsegmentDuration = value;
    }

    /**
     * Gets a map that contains attributes that aren't bound to any typed property on this class.
     * 
     * <p>
     * the map is keyed by the name of the attribute and 
     * the value is the string value of the attribute.
     * 
     * the map returned by this method is live, and you can add new attribute
     * by updating the map directly. Because of this design, there's no setter.
     * 
     * 
     * @return
     *     always non-null
     */
    public Map<QName, String> getOtherAttributes() {
        return otherAttributes;
    }

}
